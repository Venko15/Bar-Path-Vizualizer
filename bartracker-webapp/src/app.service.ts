import { Injectable } from '@nestjs/common';
import { InjectModel } from '@nestjs/mongoose';
import { Path, PathDocument } from './schemas/path.schema';
import { Model } from 'mongoose';

@Injectable()
export class AppService {
  constructor(@InjectModel(Path.name) private pathModel: Model<PathDocument>) {}

  async create(pathData: any): Promise<Path> {

    const createdPath = new this.pathModel({values:pathData});
    return createdPath.save();
  }

  async findAll(): Promise<Path[]> {
      return this.pathModel.find().exec();
    }
}
